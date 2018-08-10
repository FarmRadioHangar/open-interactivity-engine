import * as types from './actionTypes';  
import api from '../api';

export function loadCampaigns() {  
  return function(dispatch) {
    return api.get('campaigns')
      .then(response => {
        dispatch(loadCampaignsSuccess(response.campaigns));
      })
      .catch(error => {
        throw(error);
      });
  };
}

export function loadCampaignsSuccess(campaigns) {  
  return {type: types.LOAD_CAMPAIGNS_SUCCESS, campaigns};
}
