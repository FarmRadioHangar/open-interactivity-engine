import { fork, all, call, put, takeEvery } from 'redux-saga/effects';
import * as types from '../actions/actionTypes';  
import * as languagesActions from '../actions/languagesActions';
import api from '../api';

function* callGetLanguagesSaga(action) {
  try {
    const { offset, limit } = action;
    const response = yield call(api.get, 'languages', { skip: offset, limit });
    if (response.ok) {
      yield put(languagesActions.fetchLanguagesDone(response));
    } else {
      yield put(languagesActions.fetchLanguagesError(`API error: ${response.error}`));
    }
  } catch(err) {
    yield put(languagesActions.fetchLanguagesError('Could not connect to API server.'));
  }
}

function* getLanguagesSaga() {
  yield takeEvery(types.FETCH_LANGUAGES, callGetLanguagesSaga);
}

function* callPostLanguageSaga(action) {
  const { data } = action;
  console.log('Data : --------------------');
  console.log(data);

  const response = yield call(api.post, 'languages', data);

  try {
    //
  } catch(err) {
  }
}
 
function* postLanguageSaga() {
  yield takeEvery(types.CREATE_LANGUAGE, callPostLanguageSaga);
}

export function* rootSaga() {
  yield all([
    fork(getLanguagesSaga),
    fork(postLanguageSaga)
  ]);
}
