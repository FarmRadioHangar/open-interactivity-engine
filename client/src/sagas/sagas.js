import formActionSaga from 'redux-form-saga';
import { SubmissionError } from 'redux-form';
import { fork, all, call, put, select, takeEvery } from 'redux-saga/effects';
import { router } from 'redux-saga-router';
import * as actions from '../actions/creators';
import history from '../history';
import Api from '../api';

const api = new Api();

const delay = (ms) => new Promise(res => setTimeout(res, ms));

export function* rootSaga() {
  yield all([
    fork(deleteLanguageSaga),
    fork(createLanguageFormSaga),
    fork(updateLanguageFormSaga),
    fork(formActionSaga),
    fork(router, history, routes)
  ]);
}

function* callDeleteLanguage(action) {
  let tasks;
  try {
    tasks = yield all([
      call(::api.httpDelete, `languages/${action.id}`),
      delay(300)
    ]);
  } catch(error) {
    yield put(actions.deleteLanguageFailure(error));
    return;
  }
  const response = tasks[0];
  if (response.ok) {
    yield put(actions.deleteLanguageSuccess());
    yield call([history, 'push'], '/languages');
  } else {
    yield put(actions.deleteLanguageFailure(Error('Language was not deleted.')));
  }
}

function* deleteLanguageSaga() {
  yield takeEvery('DELETE_LANGUAGE_REQUEST', callDeleteLanguage);
}

function* callPostLanguage(action) {
  const data = Api.toSnakeCase(action.payload);
  let tasks;
  try {
    tasks = yield all([
      call(::api.post, 'languages', data),
      delay(300)
    ]);
  } catch(error) {
    let options = { _error: error.message };
    yield put(actions.createLanguageAction.failure(new SubmissionError(options)));
    return;
  }
  const response = tasks[0];
  if (response.ok) {
    yield put(actions.createLanguageAction.success());
    yield call([history, 'push'], '/languages');
  } else {
    let options = { _error: 'Language was not saved.' };
    switch(response.code) {
      case 'UNIQUE_CONSTRAINT_VIOLATION': {
        options[response.key] = 'The name must be unique. A language with this name already exists.';
        break;
      }
    }
    yield put(actions.createLanguageAction.failure(new SubmissionError(options)));
  }
}

function* createLanguageFormSaga() {
  yield takeEvery(actions.createLanguageAction.REQUEST, callPostLanguage);
}

function* callPutLanguage(action) {
  const id = action.payload._id['$oid'];
  const data = Api.toSnakeCase(action.payload);
  let tasks;
  try {
    tasks = yield all([
      call(::api.put, `languages/${id}`, data),
      delay(300)
    ]);
  } catch(error) {
    let options = { _error: error.message };
    yield put(actions.updateLanguageAction.failure(new SubmissionError(options)));
    return;
  }
  const response = tasks[0];
  if (response.ok) {
    yield put(actions.updateLanguageAction.success());
    yield call([history, 'push'], '/languages'); // TODO redirect to view
  } else {
    let options = { _error: 'Language was not updated.' };
    switch(response.code) {
      case 'UNIQUE_CONSTRAINT_VIOLATION': {
        options[response.key] = 'The name must be unique. A language with this name already exists.';
        break;
      }
    }
    yield put(actions.updateLanguageAction.failure(new SubmissionError(options)));
  }
}

function* updateLanguageFormSaga() {
  yield takeEvery(actions.updateLanguageAction.REQUEST, callPutLanguage);
}

function* fetchPage(resource, page = 1) {
  const state = yield select();
  const { pageSize } = state[resource];
  const offset = pageSize*(page - 1);
  yield all([
    put(actions.fetchPageRequest(resource, offset)),
    delay(300)
  ]);
  let response;
  try {
    response = yield call(::api.get, resource, { skip: offset, limit: pageSize });
  } catch(error) {
    const message = `Error fetching ${resource}. Is the server running and accepting connections?`;
    yield put(actions.fetchPageFailure(resource, new Error(message)));
    return;
  }
  if (response.ok) {
    const items = Api.toCamelCase(response[resource]);
    yield put(actions.fetchPageSuccess(resource, items, response.total));
  } else {
    yield put(actions.fetchItemFailure(resource, new Error(response.error)));
  }
}

function* fetchItem(resource, id, process) {
  yield all([
    put(actions.fetchItemRequest(resource)),
    delay(300)
  ]);
  let response;
  try {
    response = yield call(::api.get, `languages/${id}`);
  } catch(error) {
    yield put(actions.fetchItemFailure(resource, error));
    return;
  }
  if (response.ok) {
    if ('function' === typeof process) {
      yield process(response);
    }
  } else {
    if (404 == response.status) {
      yield put(actions.fetchItemFailure(resource, new Error('This document doesn\'t exist.')));
    } else {
      yield put(actions.fetchItemFailure(resource, new Error(response.error)));
    }
  }
}

const routes = {
  '/languages': function* (params) {
    yield* fetchPage('languages');
  },
  '/languages/page/:page': function* (params) {
    yield* fetchPage('languages', params.page);
  },
  '/languages/:id/view': function* (params) { 
    yield* fetchItem('languages', params.id, function* (response) {
      const data = Api.toCamelCase(response.language);
      yield put(actions.fetchItemSuccess('languages', data));
    });
  },
  '/languages/:id/delete': function* (params) { 
    yield* fetchItem('languages', params.id, function* (response) {
      const data = Api.toCamelCase(response.language);
      yield put(actions.fetchItemSuccess('languages', data));
    });
  },
  '/languages/:id/edit': function* (params) { 
    yield* fetchItem('languages', params.id, function* (response) {
      const data = Api.toCamelCase(response.language);
      yield put(actions.fetchItemSuccess('languages', data));
      yield put({
        type: '@@redux-form/INITIALIZE',
        meta: { form: 'languages-update' },
        payload: { ...data }
      });
    });
  },
  '/audience': function* (params) {
    yield* fetchPage('audience');
  },
  '/audience/page/:page': function* (params) {
    yield* fetchPage('audience', params.page);
  },
  '/audience/:id/view': function* (params) { },
  '/audience/:id/edit': function* (params) { },
  '/audience/:id/delete': function* (params) { },
  '/content': function* (params) {
    yield* fetchPage('content');
  },
  '/content/page/:page': function* (params) {
    yield* fetchPage('content', params.page);
  },
  '/content/:id/view': function* (params) { },
  '/content/:id/edit': function* (params) { },
  '/content/:id/delete': function* (params) { },
  '/campaigns': function* (params) {
    yield* fetchPage('campaigns');
  },
  '/campaigns/page/:page': function* (params) {
    yield* fetchPage('campaigns', params.page);
  },
  '/campaigns/:id/view': function* (params) { },
  '/campaigns/:id/edit': function* (params) { },
  '/campaigns/:id/delete': function* (params) { }
};
