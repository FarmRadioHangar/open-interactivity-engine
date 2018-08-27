import React from 'react';
import { reduxForm } from 'redux-form';
import { LanguagesForm, validate, warn } from './form';
import * as actions from '../../actions/creators';

const Form = (props) => (
  <form onSubmit={props.handleSubmit(actions.createLanguageAction)}>
    <LanguagesForm {...props} />
  </form>
);

export default reduxForm({ form: 'languages-create', validate, warn })(Form);
