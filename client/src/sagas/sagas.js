import { fork, all, call, put, takeEvery } from 'redux-saga/effects';
import * as types from '../actions/actionTypes';  
import * as languagesActions from '../actions/languagesActions';
import * as utils from '../utils';
import history from '../history';
import api from '../api';

const delay = (ms) => new Promise(res => setTimeout(res, ms));

function* callGetLanguagesSaga(action) {
  try {
    const { offset, limit } = action;
    const response = yield call(api.get, 'languages', { skip: offset, limit });
    if (response.ok) {
      const { count, total } = response;
      const items = response.languages.map(lang => utils.translKeys(lang, true));
      yield put(languagesActions.fetchLanguagesDone(items, count, total));
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
  try {
    const { data } = action;
    const response = yield call(api.post, 'languages', utils.translKeys(data));
    response.payload = utils.translKeys(response.payload, true);
    if (response.ok) {
      yield delay(350);
      yield put(languagesActions.createLanguageDone());
      yield call([history, 'push'], '/languages');
    } else {
      yield delay(350);
      yield put(languagesActions.createLanguageError(`API error: ${response.error}`));
      action.reject();
    }
  } catch(err) {
    yield delay(350);
    yield put(languagesActions.createLanguageError('Could not connect to API server.'));
    action.reject();
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
