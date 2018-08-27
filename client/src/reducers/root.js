import { combineReducers } from 'redux';
import { reducer as formReducer } from 'redux-form'
import languages from './languages';
import content from './content';
import audience from './audience';
import campaigns from './campaigns';
import debug from './debug';

export default combineReducers({
  languages,
  content,
  audience,
  campaigns,
  debug,
  form: formReducer
});
