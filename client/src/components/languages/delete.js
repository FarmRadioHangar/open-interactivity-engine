import React from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';
import * as actions from '../../actions/creators';

const LanguagesDelete = ({ item, itemFetching, itemError, dispatch }) => {
  const id = item && item._id['$oid'];
  return (
    <React.Fragment>
      {itemError ? (
        <React.Fragment>
          {itemError.message}
        </React.Fragment>
      ) : (
        <React.Fragment>
          {itemFetching ? (
            <React.Fragment>
              Please wait...
            </React.Fragment>
          ) : (
            <React.Fragment>
              <h2>{item.name}</h2>
              <div>
                <button onClick={() => {
                  dispatch(actions.deleteLanguageRequest(id));
                }}>
                  Confirm delete
                </button>
              </div>
            </React.Fragment>
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

export default connect(mapStateToProps)(LanguagesDelete);
