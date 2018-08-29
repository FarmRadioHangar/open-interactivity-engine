import React from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';
import { reduxForm } from 'redux-form';
import { LanguagesForm, validate, warn } from './form';
import * as actions from '../../actions/creators';

const LanguagesEdit = ({ item, itemFetching, itemError, initialized, ...props }) => {
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
            <LanguagesForm submitAction={actions.updateLanguageAction} {...props} />
          )}
        </React.Fragment>
      )}
    </React.Fragment>
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

const ReduxForm = reduxForm({ form: 'languages-update', validate, warn })(LanguagesEdit);

export default connect(mapStateToProps)(ReduxForm);
