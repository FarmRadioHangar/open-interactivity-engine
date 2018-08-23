import * as types from './actionTypes';

export function fetchLanguages(offset, limit) {
  return { type: types.FETCH_LANGUAGES, offset, limit };
}
