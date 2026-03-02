class Secrets {
  static const String googleClientId = '736605041162-l9hfp01tnje5372th5bm3jbnbsgh0cfp.apps.googleusercontent.com';
  static const String googleRedirectUri = 'http://localhost:8080/api/auth/google/callback'; // or appropriate scheme

  static const String dropboxAppKey = 'wzbnv9lrpde4p61';
  static const String dropboxRedirectUri = 'http://localhost:8080/api/auth/dropbox/callback'; // Standardize this
  
  // LinkedIn - Using value found in backend .env
  static const String linkedinClientId = '78edkj5jnby6is'; 
  static const String linkedinRedirectUri = 'http://localhost:8080/api/auth/linkedin/callback/';

  static const String githubClientId = 'Ov23liL8bhCWdMXAF2dx';
  static const String githubRedirectUri = 'http://localhost:8080/api/auth/github/callback/';

  static const String gitlabClientId = '1abc545a7deaa25ba642aa28b21171947521a4271634fe38a6cb9b707b724f8f';
  static const String gitlabRedirectUri = 'http://localhost:8080/api/auth/gitlab/callback/';
}
