import { fork, all, call, put, takeEvery } from 'redux-saga/effects';
import * as types from '../actions/actionTypes';  
import api from '../api';
import * as languagesActions from '../actions/languagesActions';

function* callGetLanguagesSaga(action) {
  try {
    const response = yield call(api.get, 'languages', {});
    if (response.ok) {
      yield put(languagesActions.fetchLanguagesDone(response));
    } else {
      yield put(languagesActions.fetchLanguagesFailed(`API error: ${response.error}`));
    }
  } catch(err) {
    yield put(languagesActions.fetchLanguagesFailed('Could not connect to API server.'));
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
