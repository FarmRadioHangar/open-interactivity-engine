import formActionSaga from 'redux-form-saga';
import { SubmissionError } from 'redux-form';
import { fork, all, call, put, select, takeLatest, takeEvery } from 'redux-saga/effects';
import { router } from 'redux-saga-router';
import { createLanguageAction } from '../actions/languages';
import * as languagesActions from '../actions/languages';
import * as types from '../actions/types';
import history from '../history';
import Api from '../api';

const api = new Api();

const delay = (ms) => new Promise(res => setTimeout(res, ms));

function* callGetLanguagesSaga(action) {
  try {
    const { offset, limit } = action;
    const response = yield call(::api.get, 'languages', { skip: offset, limit });

    if (response.ok) {
      const { languages, count, total } = response;
      yield put(languagesActions.fetchLanguagesSuccess(Api.toCamelCase(languages), count, total));
    } else {
      //
    }

  } catch(error) {
    yield put(languagesActions.fetchLanguagesFailure('Could not fetch'));
  }
}

function* getLanguagesSaga() {
  yield takeEvery(types.FETCH_LANGUAGES, callGetLanguagesSaga);
}

function* callPostLanguage(action) {
  try {
    const data = Api.toSnakeCase(action.payload);
    yield delay(350);
    const response = yield call(::api.post, 'languages', data);
    if (response.ok) {
      yield put(createLanguageAction.success());
      yield call([history, 'push'], '/languages');
      console.log(Api.toCamelCase(response.language));
    } else {
      let options = { _error: 'Language was not saved.' };
      switch(response.code) {
        case 'UNIQUE_CONSTRAINT_VIOLATION':
          options[response.key] = 'The name must be unique. A language with this name already exists.';
          break;
        default:
          break;
      }
      yield put(createLanguageAction.failure(new SubmissionError(options)));
    }
  } catch(error) {
    let options = { _error: 'Error communicating with API. Is the server running and accepting connections?' };
    yield put(createLanguageAction.failure(new SubmissionError(options)));
  }
}

function* createLanguageSaga() {
  yield takeEvery(createLanguageAction.REQUEST, callPostLanguage);
}

const routes = {
  '/languages/page/:page': function*(params) {
    const state = yield select();
    const { total, pageSize } = state.languages;
    const lastPage = Math.ceil(total/pageSize);
    const offset = pageSize*(params.page - 1);
    yield put(languagesActions.fetchLanguages(offset, pageSize));
  },
  '/languages': function*(params) {
    const state = yield select();
    const { pageSize } = state.languages;
    yield put(languagesActions.fetchLanguages(0, pageSize));
  }
};

export function* rootSaga() {
  yield all([
    fork(getLanguagesSaga),
    fork(createLanguageSaga),
    fork(formActionSaga),
    fork(router, history, routes)
  ]);
}
