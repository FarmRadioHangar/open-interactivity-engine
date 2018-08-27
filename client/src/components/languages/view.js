import React from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';

const LanguagesView = ({ item, itemFetching, itemError }) => {
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
            <React.Fragment>
              <div>
                Name: {item.name}
              </div>
              <div>
                Tag: {item.tag}
              </div>
            </React.Fragment>
          )}
        </React.Fragment>
      )}
    </React.Fragment>
  );
};

LanguagesView.propTypes = {
  item: PropTypes.object,
  itemFetching: PropTypes.bool,
  itemError: PropTypes.object
};

function mapStateToProps(state, ownProps) {
  return state.languages;
}

export default connect(mapStateToProps)(LanguagesView);
