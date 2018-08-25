import 'babel-polyfill';

import React from 'react';
import ReactDOM from 'react-dom';
import { Field, reduxForm } from 'redux-form'
import { Link } from 'react-router-dom';
import { Provider, connect } from 'react-redux';
import { Route, Switch } from 'react-router';
import { Router } from 'react-router-dom';
import { rootSaga } from './sagas/sagas';
import sagaMiddleware from './sagas/middleware';
import configureStore from './store/configure';
import history from './history';
import * as languagesActions from './actions/languages';
import LanguagesList from './components/languagesList';
import Pagination from './components/pagination';

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
      <Route exact path='/languages/page/:page' component={LanguagesComponent} />
      <Route exact path='/languages/create' component={CreateLanguageComponent} />
      <Route exact path='/languages/:id/view' component={ShowLanguage} />
      <Route exact path='/languages/:id/edit' component={EditLanguage} />
      <Route exact path='/languages/:id/delete' component={DeleteLanguage} />
      <Route render={() => (<div>Nothing</div>)} />
    </Switch>
  </div>
);

const DeleteLanguage = () => {
  return (
    <span />
  );
}

const EditLanguage = () => {
  return (
    <span />
  );
}

const ShowLanguage= () => {
  return (
    <span />
  );
}

const CreateLanguageComponent = () => {
  return (
    <LanguageFormComponent />
  );
}

const RenderField = ({ input, label, type, meta: { touched, error, warning } }) => (
  <div>
    <label>{label}</label>
    <div>
      <input {...input} placeholder={label} type={type} />
    </div>
    <div>
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
      {submitting && (
        <div>
          Please wait...
        </div>
      )}
      <form onSubmit={handleSubmit(languagesActions.createLanguageAction)}>
        <div>
          <div>
            <label htmlFor='name'>Name</label>
          </div>
          <div>
            <Field name='name' component={RenderField} type='text' />
          </div>
        </div>
        <div>
          <div>
            <label htmlFor='tag'>Tag</label>
          </div>
          <div>
            <Field name='tag' component={RenderField} type='text' />
          </div>
        </div>
        <div>
          <button type='submit' disabled={submitting}>Submit</button>
        </div>
      </form>
    </div>
  );
}

const validate = (values) => {
  const errors = {};
  if (!values.name) {
    errors.name = 'This field is required.'
  }
  if (!values.tag) {
    errors.tag = 'This field is required.'
  }
  if (values.tag && values.tag.length < 2) {
    errors.tag = 'The language tag must be at least two characters long.'
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
  return (
    <div>
      <div>
        Languages
      </div>
      <div>
        <button onClick={() => { history.push('/languages/create'); }}>
          Create language
        </button>
      </div>
      {languages.error ? (
        <div>
          Error: {languages.error}
          <button onClick={() => { history.push(history.location); }}>
            Try again!
          </button>
        </div>
      ) : (
        <div>
          <div>
            <LanguagesList languages={languages} />
          </div>
          <div>
            <Pagination
              pageSize = {languages.pageSize}
              total    = {languages.total}
              offset   = {languages.offset}
              onChange = {page => { history.push(`/languages/page/${page}`); }}
            />
          </div>
        </div>
      )}
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
    <Router history={history}>
      <Main />
    </Router>
  </Provider>,
  document.getElementById('app')
);
