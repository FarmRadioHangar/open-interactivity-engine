import React from 'react';
import PropTypes from 'prop-types'
import { Link } from 'react-router-dom'

const CampaignsList = ({campaigns}) => {
  return (
    <ul className='list-group'>
      {campaigns.map(campaign => {
        const id = campaign['_id']['$oid'];
        return (
          <li className='list-group-item' key={id}>
            <span>{campaign.name}</span>
            <Link to={`/campaigns/${id}/edit`}>Edit</Link>
            <Link to={`/campaigns/${id}/delete`}>Delete</Link>
          </li>
        );
      })}
    </ul>
  );
};

CampaignsList.propTypes = {
  campaigns: PropTypes.array.isRequired
};

export default CampaignsList;
