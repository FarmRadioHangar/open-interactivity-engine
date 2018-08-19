import React from 'react';
import PropTypes from 'prop-types'
import { connect } from 'react-redux';
import { Link } from 'react-router-dom'
import * as languagesActions from '../actions/languagesActions';
import LanguagesList from './languagesList';
import history from '../history';
import Pagination from './pagination';

class LanguagesIndex extends React.Component {
  componentDidMount() {
    this.props.dispatch(languagesActions.fetchLanguages());
  }
  render() {
    const { languages, dispatch } = this.props;
    return(
      <span>
        <LanguagesList
          languages = {languages}
          onReload  = {() => {
            dispatch(languagesActions.fetchLanguages());
          }}
        />
        <Link to='/languages/create'>Add language</Link>
        <Pagination
          pageSize = {languages.pageSize}
          total    = {languages.total}
          offset   = {languages.offset}
          onChange = {pageNumber => {
            history.push(`/languages/page/${pageNumber}`);
          }}
        />
      </span>
    );
  }
}

LanguagesIndex.propTypes = {
  languages: PropTypes.object.isRequired
};

function mapStateToProps(state, ownProps) {
  return {
    languages: state.languages
  };
}

export default connect(mapStateToProps)(LanguagesIndex);
