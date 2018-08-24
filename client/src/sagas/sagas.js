import formActionSaga from 'redux-form-saga';
import { SubmissionError } from 'redux-form';
import { fork, all, call, put, takeLatest, takeEvery } from 'redux-saga/effects';
import { createLanguageAction } from '../actions/languages';
import * as languagesActions from '../actions/languages';
import * as types from '../actions/types';
import api from '../api';

const delay = (ms) => new Promise(res => setTimeout(res, ms));

function* callGetLanguagesSaga(action) {
  try {
    const { offset, limit } = action;
    const response = yield call(::api.get, 'languages', { skip: offset, limit });

    if (response.ok) {
      const { languages, count, total } = response;
      const items = languages;
      yield put(languagesActions.fetchLanguagesSuccess(items, count, total));
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
    const response = yield call(::api.post, 'languages', action.payload);
    if (!response.ok) {
      throw 'bananas';
    }
    yield put(createLanguageAction.success());
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
