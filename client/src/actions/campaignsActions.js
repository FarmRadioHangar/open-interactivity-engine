import * as types from './actionTypes';  
import history from '../history';
import api from '../api';

export function createCampaign(data) {
  return function(dispatch) {
    return api.post('campaigns', data)
      .then(response => {
        history.push('/campaigns')
        //dispatch(createCampaignsSuccess());
      })
      .catch(error => {
        throw(error);
      });
  };
}

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
