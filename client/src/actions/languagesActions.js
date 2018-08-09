import * as types from './actionTypes';  
import api from '../api';

//export function createLanguage() {
//  return function(dispatch) {
//    return api.getLanguages()
//      .then(response => {
//        dispatch(loadLanguagesSuccess(response.languages));
//      })
//      .catch(error => {
//        throw(error);
//      });
//  };
//}

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
