import React from 'react';
import { Field } from 'redux-form';

const LanguagesField = ({ input, label, type, meta }) => {
  const { touched, error, warning } = meta;
  return (
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
}

export const LanguagesForm = ({ submitAction, handleSubmit, pristine, reset, submitting, error }) => {
  return (
    <form onSubmit={handleSubmit(submitAction)}>
      <div>
        {error && (
          <div style={{border: '1px solid red'}}>
            {error}
          </div>
        )}
        {submitting && (
          <div>
            Please wait...
          </div>
        )}
        <div>
          <div>
            <label htmlFor='name'>Name</label>
          </div>
          <div>
            <Field name='name' component={LanguagesField} type='text' />
          </div>
        </div>
        <div>
          <div>
            <label htmlFor='tag'>Tag</label>
          </div>
          <div>
            <Field name='tag' component={LanguagesField} type='text' />
          </div>
        </div>
        <div>
          <button type='submit' disabled={submitting}>Submit</button>
        </div>
      </div>
    </form>
  );
};

export function validate(values) {
  let errors = {};
  if (!values.name) {
    errors.name = 'This field is required.'
  }
  if (!values.tag) {
    errors.tag = 'This field is required.'
  } else if (values.tag.length < 2) {
    errors.tag = 'The language tag must be at least two characters long.'
  }
  return errors;
}

export function warn(values) {
  let warnings = {};
  return warnings;
}
