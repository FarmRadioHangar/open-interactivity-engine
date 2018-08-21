import * as types from '../actions/actionTypes';  
import initialState from './initialState';

export default function(state = initialState.languages, action) {  
  console.log(action);
  switch(action.type) {
    case types.FETCH_LANGUAGES: {
      const { offset, limit } = action;
      return { ...state, offset, limit };
    }
    case types.FETCH_LANGUAGES_ERROR: {
      const { items, count, total } = initialState.languages;
      return { ...state, items, count, total, error: action.error };
    }
    case types.FETCH_LANGUAGES_DONE: {
      const { items, count, total } = action;
      return { ...state, items, count, total, error: false };
    }
    default: 
      return state;
  }
}
