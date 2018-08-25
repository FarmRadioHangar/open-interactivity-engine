import * as types from '../actions/types';
import initialState from './initialState';

export default function(state = initialState.languages, action) {
  console.log(action);
  switch(action.type) {
    case types.FETCH_LANGUAGES: {
      const { offset, limit } = action;
      return { ...state, offset, limit, error: false };
    }
    case types.FETCH_LANGUAGES_SUCCESS: {
      const { items, count, total } = action;
      return { ...state, items, count, total, error: false };
    }
    case types.FETCH_LANGUAGES_FAILURE: {
      const { error } = action;
      const { items, count, total } = initialState.languages;
      return { ...state, error };
    }
    default:
      return state;
  }
}
