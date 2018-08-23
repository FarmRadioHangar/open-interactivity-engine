import { combineReducers } from 'redux';
import { reducer as formReducer } from 'redux-form'
import languages from './languages';

export default combineReducers({
  languages,
  form: formReducer
});
