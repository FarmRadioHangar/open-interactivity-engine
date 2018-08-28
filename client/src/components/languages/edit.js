import React from 'react';
import { reduxForm } from 'redux-form';
import { LanguagesForm, validate, warn } from './form';
import * as actions from '../../actions/creators';

const Form = ({ handleSubmit, initialized, ...props }) => (
  <form onSubmit={handleSubmit(actions.updateLanguageAction)}>
    {initialized ? (
      <LanguagesForm {...props} />
    ) : (
      <React.Fragment>
        Please wait...
      </React.Fragment>
    )}
  </form>
);

export default reduxForm({ form: 'languages-update', validate, warn })(Form);
