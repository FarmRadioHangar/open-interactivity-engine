import { fork, all, call, put, takeEvery } from 'redux-saga/effects';
import * as types from '../actions/actionTypes';  
import api from '../api';

function* callGetLanguagesSaga(action) {
  try {
    const response = yield call(api.get, 'languages', {});
    console.log(response);
    yield put({ 
      type: types.FETCH_LANGUAGES_DONE, 
      items: response.languages,
      count: response.count,
      total: response.total
    });
  } catch(err) {
    yield put({ type: types.FETCH_LANGUAGES_FAILED });
  }
}

function* getLanguagesSaga() {
  yield takeEvery(types.FETCH_LANGUAGES, callGetLanguagesSaga);
}

export function* rootSaga() {
  yield all([
    fork(getLanguagesSaga)
  ]);
}
