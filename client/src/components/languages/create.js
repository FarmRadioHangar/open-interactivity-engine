import React from 'react';
import { reduxForm } from 'redux-form';
import { LanguagesForm, validate, warn } from './form';
import * as actions from '../../actions/creators';

const LanguagesCreate = ({ ...props }) => {
  return (
    <LanguagesForm submitAction={actions.createLanguageAction} {...props} />
  );
}

const ReduxForm = reduxForm({ form: 'languages-create', validate, warn })(LanguagesCreate);

export default ReduxForm;
