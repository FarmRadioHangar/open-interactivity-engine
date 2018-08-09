import React from 'react';  
import PropTypes from 'prop-types'
import { connect } from 'react-redux';  
import * as audienceActions from '../actions/audienceActions';
import AudienceList from './audienceList';

class AudienceIndex extends React.Component {  
  componentDidMount() {
    this.props.dispatch(audienceActions.loadAudience());
  }
  render() {
    return(
      <AudienceList audience={this.props.audience} />
    );
  }
}

AudienceIndex.propTypes = {
  audience: PropTypes.array.isRequired
};

function mapStateToProps(state, ownProps) {
  return {
    audience: state.audience
  };
} 

export default connect(mapStateToProps)(AudienceIndex);  
