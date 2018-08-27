import React from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';

const LanguagesDelete = ({ item, itemFetching, itemError }) => {
  return (
    <React.Fragment>
      {itemFetching ? (
        <span>Please wait...</span>
      ) : (
        <React.Fragment>
          {!itemError && (
            <div>
              <div>
                <h2>{item.name}</h2>
              </div>
              <div>
                <button>
                  Confirm delete
                </button>
              </div>
            </div>
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
