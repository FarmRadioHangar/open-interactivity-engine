import * as types from '../actions/types';
import initialState from './initialState';

export default function(state = initialState.language, action) {
  switch(action.type) {
    case types.FETCH_LANGUAGE:
      return { ...state, item: null, error: false };
    case types.FETCH_LANGUAGE_SUCCESS: {
      const { language } = action;
      return { ...state, item: language, error: false };
    }
    case types.FETCH_LANGUAGE_FAILURE: {
      const { error } = action;
      return { ...state, item: null, error };
    }
    default:
      return state;
  }
}
