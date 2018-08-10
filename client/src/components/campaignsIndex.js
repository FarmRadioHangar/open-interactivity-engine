import React from 'react';  
import PropTypes from 'prop-types'
import { connect } from 'react-redux';  
import { Link } from 'react-router-dom'
import * as campaignsActions from '../actions/campaignsActions';
import CampaignsList from './campaignsList';

class CampaignsIndex extends React.Component {  
  componentDidMount() {
    this.props.dispatch(campaignsActions.loadCampaigns());
  }
  render() {
    return(
      <span>
        <CampaignsList campaigns={this.props.campaigns} />
        <Link to='/campaigns/create'>Create a campaign</Link>
      </span>
    );
  }
}

CampaignsIndex.propTypes = {
  campaigns: PropTypes.array.isRequired
};

function mapStateToProps(state, ownProps) {
  return {
    campaigns: state.campaigns
  };
} 

export default connect(mapStateToProps)(CampaignsIndex);  
