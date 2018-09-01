import React from 'react';
import { Route, Switch } from 'react-router';
import CampaignsIndex from './components/campaigns/index';
import CampaignsCreate from './components/campaigns/create';
import CampaignsView from './components/campaigns/view';
import CampaignsEdit from './components/campaigns/edit';
import CampaignsDelete from './components/campaigns/delete';
import ContentIndex from './components/content/index';
import ContentCreate from './components/content/create';
import ContentView from './components/content/view';
import ContentEdit from './components/content/edit';
import ContentDelete from './components/content/delete';
import AudienceIndex from './components/audience/index';
import AudienceCreate from './components/audience/create';
import AudienceView from './components/audience/view';
import AudienceEdit from './components/audience/edit';
import AudienceDelete from './components/audience/delete';
import LanguagesIndex from './components/languages/index';
import LanguagesCreate from './components/languages/create';
import LanguagesView from './components/languages/view';
import LanguagesEdit from './components/languages/edit';
import LanguagesDelete from './components/languages/delete';
import Settings from './components/settings';
import NotFound from './components/notfound';

const Routes = () => (
  <Switch>
    <Route exact path='/' component={() => <div>Home</div>} />

    <Route exact path='/campaigns' component={CampaignsIndex} />
    <Route exact path='/campaigns/page/:page' component={CampaignsIndex} />
    <Route exact path='/campaigns/create' component={CampaignsCreate} />
    <Route exact path='/campaigns/:id/view' component={CampaignsView} />
    <Route exact path='/campaigns/:id/edit' component={CampaignsEdit} />
    <Route exact path='/campaigns/:id/delete' component={CampaignsDelete} />

    <Route exact path='/content' component={ContentIndex} />
    <Route exact path='/content/page/:page' component={ContentIndex} />
    <Route exact path='/content/create' component={ContentCreate} />
    <Route exact path='/content/:id/view' component={ContentView} />
    <Route exact path='/content/:id/edit' component={ContentEdit} />
    <Route exact path='/content/:id/delete' component={ContentDelete} />

    <Route exact path='/audience' component={AudienceIndex} />
    <Route exact path='/audience/page/:page' component={AudienceIndex} />
    <Route exact path='/audience/create' component={AudienceCreate} />
    <Route exact path='/audience/:id/view' component={AudienceView} />
    <Route exact path='/audience/:id/edit' component={AudienceEdit} />
    <Route exact path='/audience/:id/delete' component={AudienceDelete} />

    <Route path='/languages'>
      <Languages>
        <Route path='/languages(/page/:page)?' component={LanguagesIndex} />
      </Languages>
    </Route>

  {/*
    <Route exact path='/languages' component={LanguagesIndex} />
    <Route exact path='/languages/page/:page' component={LanguagesIndex} />
    <Route exact path='/languages/create' component={LanguagesCreate} />
    <Route exact path='/languages/:id/view' component={LanguagesView} />
    <Route exact path='/languages/:id/edit' component={LanguagesEdit} />
    <Route exact path='/languages/:id/delete' component={LanguagesDelete} />
    */}

    <Route exact path='/settings' component={Settings} />
    <Route component={NotFound} />
  </Switch>
);

const Languages = (props) => {
  return (
    <div>
      {props.children}
    </div>
  );
};

export default Routes;
