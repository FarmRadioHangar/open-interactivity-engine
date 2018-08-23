import { fork, all, call, put, takeEvery } from 'redux-saga/effects';
import * as types from '../actions/actionTypes';  
import api from '../api';

const delay = (ms) => new Promise(res => setTimeout(res, ms));

function* callGetLanguagesSaga(action) {
}

function* getLanguagesSaga() {
  yield takeEvery(types.FETCH_LANGUAGES, callGetLanguagesSaga);
}

export function* rootSaga() {
  yield all([
    fork(getLanguagesSaga)
  ]);
}
