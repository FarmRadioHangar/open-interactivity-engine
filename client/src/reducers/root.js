import { combineReducers } from 'redux';
import { reducer as formReducer } from 'redux-form'
import languages from './languages';
import language from './language';

export default combineReducers({
  languages,
  language,
  form: formReducer
});
