import React from 'react';
import PropTypes from 'prop-types'

const CampaignsList = ({campaigns}) => {  
  return (
    <ul className='list-group'>
      {campaigns.map(campaign =>
        <li className='list-group-item' key={campaign['_id']['$oid']}>
          {campaign.name}
        </li>
      )}
    </ul>
  );
};

CampaignsList.propTypes = {  
  campaigns: PropTypes.array.isRequired
};

export default CampaignsList; 
