import React from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';
import { Field, reduxForm } from 'redux-form';
import * as actions from '../../actions/creators';

const Form = ({ item, itemFetching, itemError, handleSubmit, initialized, submitting, error, ...props }) => {
  return (
    <form onSubmit={handleSubmit(actions.deleteLanguageAction)}>
      {itemFetching ? (
        <React.Fragment>
          Please wait...
        </React.Fragment>
      ) : (
        <React.Fragment>
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
        </React.Fragment>
      )}
    </form>
  );
};

const validate = (values) => {
  return {};
};

const LanguagesDelete = (props) => {
  const id = props.item && props.item._id['$oid'];
  if (props.itemError) {
    return (
      <React.Fragment>
        {props.itemError.message}
      </React.Fragment>
    );
  } else {
    const ReduxForm = reduxForm({
      form: 'languages-delete',
      initialValues: { id },
      validate
    })(Form);
    return (
      <React.Fragment>
        <ReduxForm {...props} />
      </React.Fragment>
    );
  }
};

LanguagesDelete.propTypes = {
  item: PropTypes.object,
  itemFetching: PropTypes.bool,
  itemError: PropTypes.object
};

function mapStateToProps(state, ownProps) {
  return state.languages;
}

export default connect(mapStateToProps)(LanguagesDelete);
