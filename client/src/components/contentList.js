import React from 'react';
import PropTypes from 'prop-types'

const ContentList = ({content}) => {  
  return (
    <ul className='list-group'>
      {content.map(item =>
        <li className='list-group-item' key={item['_id']['$oid']}>
          {item.name}
        </li>
      )}
    </ul>
  );
};

ContentList.propTypes = {  
  content: PropTypes.array.isRequired
};

export default ContentList; 
