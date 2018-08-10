import * as types from './actionTypes';  
import api from '../api';

export function loadContent() {  
  return function(dispatch) {
    return api.get('content')
      .then(response => {
        dispatch(loadContentSuccess(response.content));
      })
      .catch(error => {
        throw(error);
      });
  };
}

export function loadContentSuccess(content) {  
  return {type: types.LOAD_CONTENT_SUCCESS, content};
}
