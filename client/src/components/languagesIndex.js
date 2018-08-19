import React from 'react';  
import PropTypes from 'prop-types'
import { connect } from 'react-redux';  
import { Link } from 'react-router-dom'
import * as languagesActions from '../actions/languagesActions';
import LanguagesList from './languagesList';

class LanguagesIndex extends React.Component {  
  componentDidMount() {
    this.props.dispatch(languagesActions.fetchLanguages());
  }
  render() {
    return(
      <span>
        <LanguagesList 
          languages={this.props.languages} 
          onReload={() => {
            this.props.dispatch(languagesActions.fetchLanguages());
          }}
        />
        <Link to='/languages/create'>Add language</Link>
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
