import * as types from '../actions/actionTypes';  
import initialState from './initialState';

export default function(state = initialState.audience, action) {  
  switch(action.type) {
    case types.LOAD_AUDIENCE_SUCCESS:
      return action.audience;
    default: 
      return state;
  }
}
