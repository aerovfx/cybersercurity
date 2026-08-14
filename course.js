const content = document.querySelector('#lesson-content');
const toc = document.querySelector('#toc');
const headings = [...content.querySelectorAll('h2, h3')];

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

