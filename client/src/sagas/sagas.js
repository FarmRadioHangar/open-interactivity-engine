import { fork, all, call, put, takeLatest } from 'redux-saga/effects';
import * as types from '../actions/actionTypes';  
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

export function* rootSaga() {
  yield all([
    fork(getLanguagesSaga)
  ]);
}
