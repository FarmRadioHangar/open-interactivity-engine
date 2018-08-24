import * as types from '../actions/types';
import initialState from './initialState';

export default function(state = initialState.languages, action) {
  console.log(action);
  switch(action.type) {
    case types.FETCH_LANGUAGES_SUCCESS: {
      const { items, count, total } = action;
      return { ...state, items, count, total }; // , error: false
    }
    default:
      return state;
  }
}
