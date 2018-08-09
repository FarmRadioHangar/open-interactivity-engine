import * as types from '../actions/actionTypes';  
import initialState from './initialState';

export default function(state = initialState.languages, action) {  
  switch(action.type) {
    case types.LOAD_LANGUAGES_SUCCESS:
      return action.languages;
    default: 
      return state;
  }
}
