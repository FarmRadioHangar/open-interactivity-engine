import 'babel-polyfill';

import React from 'react';
import ReactDOM from 'react-dom';
import { Provider, connect } from 'react-redux';
import { Route, Switch } from 'react-router';
import { Link } from 'react-router-dom';
import { ConnectedRouter } from 'connected-react-router';
import { Field, reduxForm } from 'redux-form'
import { rootSaga } from './sagas/sagas';
import sagaMiddleware from './sagas/middleware';
import configureStore from './store/configure';
import history from './history';
import * as languagesActions from './actions/languages';
import LanguagesList from './components/languagesList';

const store = configureStore();

sagaMiddleware.run(rootSaga);

const Main = () => (
  <div>
    <ul>
      <li>
        <Link to='/'>Home</Link>
      </li>
      <li>
        <Link to='/campaigns'>Campaigns</Link>
      </li>
      <li>
        <Link to='/content'>Content</Link>
      </li>
      <li>
        <Link to='/audience'>Audience</Link>
      </li>
      <li>
        <Link to='/languages'>Languages</Link>
      </li>
      <li>
        <Link to='/settings'>Settings</Link>
      </li>
    </ul>
    <hr />
    <Switch>
      <Route exact path='/' render={() => (<div>Home</div>)} />
      <Route exact path='/languages' component={LanguagesComponent} />
      <Route render={() => (<div>Nothing</div>)} />
    </Switch>
  </div>
);

const RenderField = ({ input, label, type, meta: { touched, error, warning } }) => (
  <div>
    <label>{label}</label>
    <div>
      <input {...input} placeholder={label} type={type} />
      {touched && ((error && <span>{error}</span>) || (warning && <span>{warning}</span>))}
    </div>
  </div>
);

const LanguageForm = (props) => {
  const { handleSubmit, pristine, reset, submitting } = props;
  return (
    <div>
      {props.error && (
        <div style={{border: '1px solid red'}}>
          {props.error}
        </div>
      )}
      <form onSubmit={handleSubmit(languagesActions.createLanguageAction)}>
        <div>
          <label htmlFor='firstName'>First Name</label>
          <Field name='firstName' component={RenderField} type='text' />
        </div>
        <div>
          <label htmlFor='lastName'>Last Name</label>
          <Field name='lastName' component='input' type='text' />
        </div>
        <div>
          <label htmlFor='email'>Email</label>
          <Field name='email' component='input' type='email' />
        </div>
        <button type='submit'>Submit</button>
      </form>
    </div>
  );
}

const validate = (values) => {
  const errors = {};
  if (!values.firstName) {
    errors.firstName= 'Required'
  }
  return errors;
}

const warn = (values) => {
  const warnings = {};
  return warnings;
}

const LanguageFormComponent = reduxForm({ 
  form: 'language',
  validate,
  warn
})(LanguageForm);

const Languages = ({ languages, match }) => {
  console.log(JSON.stringify(match));
  return (
    <div>
      <div>
        <LanguageFormComponent 
          onSubmit={() => {
            console.log('on submit');
          }}
        />
      </div>
      <div>
        Languages
      </div>
      <div>
        <LanguagesList languages={languages} />
      </div>
      <button onClick={() => {
        store.dispatch(languagesActions.fetchLanguages(0, 20));
      }}>
        Fetch languages
      </button>
    </div>
  );
}

function mapStateToProps(state, ownProps) {
  return {
    languages: state.languages
  };
}

const LanguagesComponent = connect(mapStateToProps)(Languages);

ReactDOM.render(
  <Provider store={store}>
    <ConnectedRouter history={history}>
      <Main />
    </ConnectedRouter>
  </Provider>,
  document.getElementById('app')
);
