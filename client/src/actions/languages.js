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

export function fetchLanguage(id) {
  return { type: types.FETCH_LANGUAGE, id };
}

export function fetchLanguageSuccess(language) {
  return { type: types.FETCH_LANGUAGE_SUCCESS, language };
}

export function fetchLanguageFailure(error) {
  return { type: types.FETCH_LANGUAGE_FAILURE, error };
}

export function editLanguage(id) {
  return { type: types.EDIT_LANGUAGE, id };
}

export const createLanguageAction = createFormAction(types.CREATE_LANGUAGE);
