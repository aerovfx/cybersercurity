const content = document.querySelector('#lesson-content');
const toc = document.querySelector('#toc');
const headings = [...content.querySelectorAll('h2, h3')];

const pathParts = location.pathname.split('/').filter(Boolean);
const courseIndex = pathParts.findIndex((part) => part.endsWith('-10weeks'));
if (courseIndex !== -1) {
  const courseRoot = `/${pathParts.slice(0, courseIndex + 1).join('/')}`;
  const isCrypto = courseRoot.includes('/2_Cryptography/');
  const resources = [
    ['Lesson', `${courseRoot}/lessons/week01.html`, '/lessons/'],
    ['Code', `${courseRoot}/code/week01/README.html`, '/code/'],
    ['Exercise', isCrypto ? `${courseRoot}/code/week01/14_bai_tap_thuc_hanh_bai_ve_nha_hands_on_exercises_homewo.py` : `${courseRoot}/exercises/week01/README.html`, '/exercises/'],
    ['Project', `${courseRoot}/projects/final_project.html`, '/projects/']
  ];
  const resourceNav = document.querySelector('#resource-nav');
  resources.forEach(([label, href, matcher], index) => {
    const link = document.createElement('a');
    link.href = href;
    link.innerHTML = `<b>0${index + 1}</b><span>${label}</span><i>↗</i>`;
    const cryptoExercise = label === 'Exercise' && isCrypto && location.pathname.includes('bai_tap');
    if (location.pathname.includes(matcher) || cryptoExercise) link.classList.add('active');
    resourceNav.appendChild(link);
  });
}

function slugify(text, index) {
  const slug = text.toLocaleLowerCase('vi').normalize('NFD')
    .replace(/[\u0300-\u036f]/g, '').replace(/[^a-z0-9]+/g, '-').replace(/(^-|-$)/g, '');
  return slug || `section-${index + 1}`;
}

headings.forEach((heading, index) => {
  if (!heading.id) heading.id = slugify(heading.textContent, index);
  const link = document.createElement('a');
  link.href = `#${heading.id}`;
  link.textContent = heading.textContent;
  if (heading.tagName === 'H3') link.classList.add('subheading');
  toc.appendChild(link);
});

if (!headings.length) toc.hidden = true;

const directChildren = [...content.children];
const firstSection = directChildren.findIndex((node) => node.tagName === 'H2');
if (firstSection > 0) {
  const intro = document.createElement('header');
  intro.className = 'doc-hero';
  content.insertBefore(intro, directChildren[0]);
  directChildren.slice(0, firstSection).forEach((node) => intro.appendChild(node));
}

[...content.children].forEach((node) => {
  if (node.tagName !== 'H2') return;
  const section = document.createElement('section');
  section.className = 'doc-section';
  content.insertBefore(section, node);
  let current = node;
  while (current && !(current !== node && current.tagName === 'H2')) {
    const next = current.nextElementSibling;
    section.appendChild(current);
    current = next;
  }
});

document.querySelector('#copy-link').addEventListener('click', async (event) => {
  await navigator.clipboard.writeText(location.href);
  const label = event.currentTarget.firstChild;
  const previous = label.textContent;
  label.textContent = 'COPIED ';
  setTimeout(() => { label.textContent = previous; }, 1400);
});

addEventListener('scroll', () => {
  const max = document.documentElement.scrollHeight - innerHeight;
  document.querySelector('#reading-progress').style.width = `${max ? scrollY / max * 100 : 0}%`;
}, { passive: true });
