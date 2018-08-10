import * as types from './actionTypes';  
import history from '../history';
import api from '../api';

export function createLanguage(data) {
  return function(dispatch) {
    return api.postLanguage(data)
      .then(response => {
        history.push('/languages')
        //dispatch(createLanguagesSuccess());
      })
      .catch(error => {
        throw(error);
      });
  };
}

export function loadLanguages() {  
  return function(dispatch) {
    return api.getLanguages()
      .then(response => {
        dispatch(loadLanguagesSuccess(response.languages));
      })
      .catch(error => {
        throw(error);
      });
  };
}

export function loadLanguagesSuccess(languages) {  
  return {type: types.LOAD_LANGUAGES_SUCCESS, languages};
}
