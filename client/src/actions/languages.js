import { createFormAction } from 'redux-form-saga';
import * as types from './types';

export function fetchLanguages(offset, limit) {
  return { type: types.FETCH_LANGUAGES, offset, limit };
}

export function fetchLanguagesSuccess(items, count, total) {
  return { type: types.FETCH_LANGUAGES_SUCCESS, items, count, total };
}

export function fetchLanguagesFailure(error) {
  return { type: types.FETCH_LANGUAGES_FAILURE, error };
}

export const createLanguageAction = createFormAction(types.CREATE_LANGUAGE);
