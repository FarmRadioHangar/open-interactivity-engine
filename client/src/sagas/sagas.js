import { fork, all, call, put, takeLatest, takeEvery } from 'redux-saga/effects';
import formActionSaga from 'redux-form-saga';
import * as types from '../actions/actionTypes';  
import { createLanguageAction } from '../actions/languages';
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
  console.log('***********************************');
  console.log(action);
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
