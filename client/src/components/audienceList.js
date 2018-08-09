import React from 'react';
import PropTypes from 'prop-types'

const AudienceList = ({audience}) => {  
  return (
    <ul className='list-group'>
      {audience.map(item =>
        <li className='list-group-item' key={item['_id']['$oid']}>
          {item.name}
        </li>
      )}
    </ul>
  );
};

AudienceList.propTypes = {  
  audience: PropTypes.array.isRequired
};

export default AudienceList; 
