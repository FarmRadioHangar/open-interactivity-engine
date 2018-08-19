import React from 'react';
import List from '../list';

const Pagination = ({ pageSize, total, offset, onChange }) => {
  const pageCount = Math.ceil(total/pageSize);
  const currentPage = offset/pageSize + 1;
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
