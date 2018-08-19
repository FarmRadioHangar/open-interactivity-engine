import React from 'react';
import ReactDOM from 'react-dom';
import { Provider } from 'react-redux';
import { Router, Route, Link } from 'react-router-dom';
import configureStore from './store/configureStore';
import history from './history';
import sagaMiddleware from './sagas/sagaMiddleware';
import { rootSaga } from './sagas/sagas';
import LanguagesIndex from './components/languagesIndex';

const store = configureStore();

sagaMiddleware.run(rootSaga);

const Main = () => (
  <Router history={history}>
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
      <Route exact path='/languages' component={Languages} />
      <Route exact path='/languages/page/:page' component={Languages} />
    </div>
  </Router>
);

const Languages = ({ match }) => {
  const pageNumber = Number(match.params.page) || 1;
  return (
    <div>
      <h2>Languages</h2>
      <LanguagesIndex pageNumber={pageNumber} />
    </div>
  );
}

//      <div>
//        <ul>
//          {this.props.languages.items.map((language, i) => 
//            <li key={i}>
//              {language.name}{' '}{JSON.stringify(language)}
//            </li>
//          )}
//        </ul>
//        <button onClick={() => {
//          this.props.dispatch({ type: 'FETCH_LANGUAGES' });
//        }}>
//          Load languages
//        </button>
//      </div>
//function mapStateToProps(state, ownProps) {
//  return {
//    languages: state.languages
//  };
//} 
//
//const AppComponent = connect(mapStateToProps)(App);

ReactDOM.render(
  <Provider store={store}>
    <Main />
  </Provider>,
  document.getElementById('app')
);

//import React from 'react';
//import ReactDOM from 'react-dom';
//import { Router, Route, Link } from 'react-router-dom';
//import { Provider } from 'react-redux';
//import { Formik, Form, Field } from 'formik';
//import history from './history';
//import configureStore from './store/configureStore';
//import CampaignsIndex from './components/campaignsIndex';
//import LanguagesIndex from './components/languagesIndex';
//import ContentIndex from './components/contentIndex';
//import AudienceIndex from './components/audienceIndex';
//import * as languagesActions from './actions/languagesActions';
//import * as campaignsActions from './actions/campaignsActions';
//
//const store = configureStore();
//
//const BasicExample = () => (
//  <Router history={history} >
//    <div>
//      <ul>
//        <li>
//          <Link to='/'>Home</Link>
//        </li>
//        <li>
//          <Link to='/campaigns'>Campaigns</Link>
//        </li>
//        <li>
//          <Link to='/content'>Content</Link>
//        </li>
//        <li>
//          <Link to='/audience'>Audience</Link>
//        </li>
//        <li>
//          <Link to='/languages'>Languages</Link>
//        </li>
//        <li>
//          <Link to='/settings'>Settings</Link>
//        </li>
//      </ul>
//      <hr />
//      <Route exact path='/' component={Home} />
//      <Route exact path='/campaigns' component={Campaigns} />
//      <Route exact path='/campaigns/create' component={CreateCampaign} />
//      <Route exact path='/campaigns/:id/edit' component={EditCampaign} />
//      <Route exact path='/campaigns/:id/delete' component={DeleteCampaign} />
//      <Route exact path='/content' component={Content} />
//      <Route exact path='/audience' component={Audience} />
//      <Route exact path='/languages' component={Languages} />
//      <Route exact path='/languages/create' component={CreateLanguage} />
//      <Route exact path='/languages/:id/edit' component={EditLanguage} />
//      <Route exact path='/languages/:id/delete' component={DeleteLanguage} />
//      <Route exact path='/settings' component={Settings} />
//    </div>
//  </Router>
//);
//
//const Home = () => (
//  <div>
//    <h2>Home</h2>
//  </div>
//);
//
//const Campaigns = () => (
//  <div>
//    <h2>Campaigns</h2>
//    <CampaignsIndex />
//  </div>
//);
//
//const CreateCampaign = () => (
//  <div>
//    <h2>Create a campaign</h2>
//    <Formik
//      initialValues={{
//        name: ''
//      }}
//      validate={values => {
//        let errors = {};
//        if (!values.name) {
//          errors.name = 'This field is required.';
//        }
//        return errors;
//      }}
//      onSubmit={(values) => {
//        store.dispatch(campaignsActions.createCampaign(values));
//      }}
//      render={({ errors, touched, isSubmitting }) => {
//        return isSubmitting ? (
//          <div>
//            Please wait...
//          </div>
//        ) : (
//          <Form>
//            <div>
//              <Field type='text' name='name' />
//              {touched.name && errors.name && <div>{errors.name}</div>}
//            </div>
//            <button type='submit' disabled={isSubmitting}>
//              Submit
//            </button>
//          </Form>
//        );
//      }}
//    />
//  </div>
//);
//
//const EditCampaign = () => (
//  <div>
//    <h2>Edit campaign</h2>
//    <div>
//      edit
//    </div>
//  </div>
//);
//
//const DeleteCampaign = ({ match }) => {
//  return (
//    <Formik
//      onSubmit={(values) => {
//        store.dispatch(campaignsActions.deleteCampaign(match.params.id));
//      }}
//      render={({ isSubmitting }) => {
//        return isSubmitting ? (
//          <div>
//            Please wait...
//          </div>
//        ) : (
//          <Form>
//            <h2>Delete campaign</h2>
//            <div>
//              delete campaign?
//            </div>
//            <button type='submit' disabled={isSubmitting}>
//              Confirm
//            </button>
//          </Form>
//        );
//      }}
//    />
//  );
//}
//
//
//const Content = () => (
//  <div>
//    <h2>Content</h2>
//    <ContentIndex />
//  </div>
//);
//
//const Audience = () => (
//  <div>
//    <h2>Audience</h2>
//    <AudienceIndex />
//  </div>
//);
//
//const Languages = () => (
//  <div>
//    <h2>Languages</h2>
//    <LanguagesIndex />
//  </div>
//);
//
//const CreateLanguage = () => (
//  <div>
//    <h2>Add language</h2>
//    <Formik
//      initialValues={{
//        name: '',
//        isoCode: ''
//      }}
//      validate={values => {
//        let errors = {};
//        if (!values.name) {
//          errors.name = 'This field is required.';
//        }
//        if (!values.isoCode) {
//          errors.isoCode = 'This field is required.';
//        }
//        return errors;
//      }}
//      onSubmit={(values) => {
//        store.dispatch(languagesActions.createLanguage(values));
//      }}
//      render={({ errors, touched, isSubmitting }) => {
//        return isSubmitting ? (
//          <div>
//            Please wait...
//          </div>
//        ) : (
//          <Form>
//            <div>
//              <Field type='text' name='name' />
//              {touched.name && errors.name && <div>{errors.name}</div>}
//            </div>
//            <div>
//              <Field type='text' name='isoCode' />
//              {touched.isoCode && errors.isoCode && <div>{errors.isoCode}</div>}
//            </div>
//            <button type='submit' disabled={isSubmitting}>
//              Submit
//            </button>
//          </Form>
//        );
//      }}
//    />
//  </div>
//);
//
//const EditLanguage = () => (
//  <div>
//    <h2>Edit language</h2>
//    <div>
//      edit
//    </div>
//  </div>
//);
//
//const DeleteLanguage = ({ match }) => {
//  return (
//    <Formik
//      onSubmit={(values) => {
//        store.dispatch(languagesActions.deleteLanguage(match.params.id));
//      }}
//      render={({ isSubmitting }) => {
//        return isSubmitting ? (
//          <div>
//            Please wait...
//          </div>
//        ) : (
//          <Form>
//            <h2>Delete language</h2>
//            <div>
//              delete language?
//            </div>
//            <button type='submit' disabled={isSubmitting}>
//              Confirm
//            </button>
//          </Form>
//        );
//      }}
//    />
//  );
//}
//
//const Settings = () => (
//  <div>
//    <h2>Settings</h2>
//  </div>
//);
//
//ReactDOM.render(
//  <Provider store={store}>
//    <BasicExample />
//  </Provider>,
//  document.getElementById('app')
//);
