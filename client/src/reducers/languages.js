import * as types from '../actions/actionTypes';  
import initialState from './initialState';

export default function(state = {}, action) {  
  switch(action.type) {
    case types.FETCH_LANGUAGES: {
      // ...
    }
    default:
      return state;
  }
}
