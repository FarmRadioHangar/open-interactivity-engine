import * as types from './actionTypes';
import history from '../history';

export function loadLanguages() {
  return { type: types.FETCH_LANGUAGES };
}

//export function deleteLanguage(id) {
//  return function(dispatch) {
//    return api.delete(`languages/${id}`)
//      .then(response => {
//        setTimeout(() => {
//          dispatch(deleteLanguageSuccess());
//          history.push('/languages')
//        }, 150);
//      })
//      .catch(error => {
//        throw(error);
//      });
//  };
//}
//
//export function deleteLanguageSuccess() {
//  return {type: types.DELETE_LANGUAGE_SUCCESS};
//}
//
//export function createLanguage(data) {
//  return function(dispatch) {
//    return api.post('languages', data)
//      .then(response => {
//        setTimeout(() => {
//          dispatch(createLanguageSuccess());
//          history.push('/languages')
//        }, 150);
//      })
//      .catch(error => {
//        throw(error);
//      });
//  };
//}
//
//export function createLanguageSuccess() {
//  return {type: types.CREATE_LANGUAGE_SUCCESS};
//}
//
//export function loadLanguages() {
//  return function(dispatch) {
//    return api.get('languages')
//      .then(response => {
//        dispatch(loadLanguagesSuccess(response.languages));
//      })
//      .catch(error => {
//        throw(error);
//      });
//  };
//}
//
//export function loadLanguagesSuccess(languages) {
//  return {type: types.LOAD_LANGUAGES_SUCCESS, languages};
//}
