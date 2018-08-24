import formActionSaga from 'redux-form-saga';
import { SubmissionError } from 'redux-form';
import { fork, all, call, put, takeLatest, takeEvery } from 'redux-saga/effects';
import { createLanguageAction } from '../actions/languages';
import * as languagesActions from '../actions/languages';
import * as types from '../actions/types';
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
    yield put(languagesActions.fetchLanguagesFailure(error));
  }
}

function* getLanguagesSaga() {
  yield takeEvery(types.FETCH_LANGUAGES, callGetLanguagesSaga);
}

function* callPostLanguage(action) {
  try {
    const response = yield call(::api.post, 'languages', Api.toSnakeCase(action.payload));
    if (!response.ok) {
      throw 'bananas';
    }
    console.log(response);
    yield put(createLanguageAction.success());
    // Api.toCamelCase(...)
  } catch(error) {
    const formError = new SubmissionError({
      firstName: 'User with this login is not found',
      _error: 'Login failed, please check your credentials and try again'
    });
    yield put(createLanguageAction.failure(formError));
    console.log('!!');
  }
}

function* createLanguageSaga() {
  yield takeEvery(createLanguageAction.REQUEST, callPostLanguage);
}

export function* rootSaga() {
  yield all([
    fork(getLanguagesSaga),
    fork(createLanguageSaga),
    fork(formActionSaga)
  ]);
}
