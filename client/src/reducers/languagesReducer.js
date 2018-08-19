import * as types from '../actions/actionTypes';  
import initialState from './initialState';

export default function(state = initialState.languages, action) {  
  console.log(action);
  switch(action.type) {
    case types.FETCH_LANGUAGES_FAILED:
      return state;
    case types.FETCH_LANGUAGES_DONE:
      const { items, count, total } = action;
      const { offset, pageSize } = state;
      return { items, count, total, offset, pageSize };
    default: 
      return state;
  }
}
