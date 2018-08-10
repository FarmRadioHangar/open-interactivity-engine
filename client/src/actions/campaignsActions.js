import * as types from './actionTypes';
import history from '../history';
import api from '../api';

export function createCampaign(data) {
  return function(dispatch) {
    return api.post('campaigns', data)
      .then(response => {
        setTimeout(() => {
          dispatch(createCampaignSuccess());
          history.push('/campaigns')
        }, 150);
      })
      .catch(error => {
        throw(error);
      });
  };
}

export function createCampaignSuccess() {
  return {type: types.CREATE_CAMPAIGN_SUCCESS};
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
