import * as types from './actionTypes';  
import api from '../api';

export function loadAudience() {  
  return function(dispatch) {
    return api.get('audience')
      .then(response => {
        dispatch(loadAudienceSuccess(response.audience));
      })
      .catch(error => {
        throw(error);
      });
  };
}

export function loadAudienceSuccess(audience) {  
  return {type: types.LOAD_AUDIENCE_SUCCESS, audience};
}
