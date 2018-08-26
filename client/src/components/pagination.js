import React from 'react';
import List from '../utils/list';

const Pagination = ({ pageSize, total, offset, onChange }) => {
  const pageCount = Math.ceil(total/pageSize);
  const currentPage = offset/pageSize + 1;
  if (pageCount < 2) { return <React.Fragment /> }
  return (
    <ul>
      {List.enumFrom(1).take(pageCount).toArray().map(page =>
        <li key={page}>
          {page == currentPage ? (
            <span>{page}</span>
          ) : (
            <a href='#' onClick={e => { e.preventDefault(); onChange(page) }}>
              {page}
            </a>
          )}
        </li>
      )}
    </ul>
  );
};

export default Pagination;
