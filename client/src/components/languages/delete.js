import React from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';
import { Field, reduxForm } from 'redux-form';
import * as actions from '../../actions/creators';

const Form = ({ item, submitAction, handleSubmit, submitting, error, ...props }) => {
  return (
    <form onSubmit={handleSubmit(submitAction)}>
      <h2>
        Delete {item.name}
      </h2>
      {submitting && (
        <p>
          Submitting
        </p>
      )}
      {error && (
        <div style={{border: '1px solid red'}}>
          {error}
        </div>
      )}
      <div>
        <button disabled={submitting} type='submit'>
          Confirm delete
        </button>
      </div>
    </form>
  );
};

const validate = (values) => {
  return {};
};

const LanguagesDelete = ({ item, itemFetching, itemError, initialized, ...props }) => {
  return (
    <React.Fragment>
      {itemFetching ? (
        <span>Please wait...</span>
      ) : (
        <React.Fragment>
          {itemError ? (
            <React.Fragment>
              {itemError.message}
            </React.Fragment>
          ) : (
            <Form {...props}
              item         = {item}
              submitAction = {actions.deleteLanguageAction}
            />
          )}
        </React.Fragment>
      )}
    </React.Fragment>
  );
};

LanguagesDelete.propTypes = {
  item: PropTypes.object,
  itemFetching: PropTypes.bool,
  itemError: PropTypes.object
};

function mapStateToProps(state, ownProps) {
  return state.languages;
}

const ReduxForm = reduxForm({ form: 'languages-delete', validate })(LanguagesDelete);

export default connect(mapStateToProps)(ReduxForm);
