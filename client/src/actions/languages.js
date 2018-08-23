import { createFormAction } from 'redux-form-saga';
import * as types from './actionTypes';

export function fetchLanguages(offset, limit) {
  return { type: types.FETCH_LANGUAGES, offset, limit };
}

export const createLanguageAction = createFormAction('CREATE_LANGUAGE');
