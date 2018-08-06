import * as types from '../actions/actionTypes';  
import initialState from './initialState';

export default function(state = initialState.campaigns, action) {  
  switch(action.type) {
    case types.LOAD_CAMPAIGNS_SUCCESS:
      return action.campaigns
    default: 
      return state;
  }
}
