import formActionSaga from 'redux-form-saga';
import { fork, all, call, put, takeLatest, takeEvery } from 'redux-saga/effects';
import { createLanguageAction } from '../actions/languages';
import * as types from '../actions/types';
import api from '../api';

const delay = (ms) => new Promise(res => setTimeout(res, ms));

function* callGetLanguagesSaga(action) {
  try {
    const { offset, limit } = action;
    const response = yield call(api.get, 'languages', { skip: offset, limit });

  } catch(error) {
    yield put({ type: types.FETCH_LANGUAGES_FAILURE, error });
  }
}

function* getLanguagesSaga() {
  yield takeLatest(types.FETCH_LANGUAGES, callGetLanguagesSaga);
}

function* callPostLanguage(action) {
  console.log(action.payload);
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
