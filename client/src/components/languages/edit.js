import React from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';
import { reduxForm } from 'redux-form';
import { LanguagesForm, validate, warn } from './form';
import * as actions from '../../actions/creators';

const Form = ({ itemFetching, itemError, handleSubmit, initialized, ...props }) => (
  <React.Fragment>
    {itemError ? (
      <React.Fragment>
        {itemError.message}
      </React.Fragment>
    ) : (
      <form onSubmit={handleSubmit(actions.updateLanguageAction)}>
        {initialized && !itemFetching ? (
          <LanguagesForm {...props} />
        ) : (
          <React.Fragment>
            Please wait...
          </React.Fragment>
        )}
      </form>
    )}
  </React.Fragment>
);

const ReduxForm = reduxForm({ form: 'languages-update', validate, warn })(Form);

const LanguagesEdit = (props) => {
  return (
    <ReduxForm {...props} />
  );
};

LanguagesEdit.propTypes = {
  item: PropTypes.object,
  itemFetching: PropTypes.bool,
  itemError: PropTypes.object
};

function mapStateToProps(state, ownProps) {
  return state.languages;
}

export default connect(mapStateToProps)(LanguagesEdit);
